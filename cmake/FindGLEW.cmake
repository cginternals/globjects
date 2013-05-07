
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY

IF(X64)

    FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h
        ${PROJECT_SOURCE_DIR}/3rdparty/glew-1.9.0-win64/include
        $ENV{TMR_HOME}/../3rdparty/glew-1.9.0-win64/include
        $ENV{GLEW_DIR}/include
        $ENV{GLEW_HOME}/include
        $ENV{PROGRAMFILES}/GLEW/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where GL/glew.h resides")

    FIND_LIBRARY(GLEW_LIBRARY
        NAMES GLEW glew glew32 glew32s
        PATHS
        ${PROJECT_SOURCE_DIR}/3rdparty/glew-1.9.0-win64/lib
        $ENV{TMR_HOME}/../3rdparty/glew-1.9.0-win64/lib
        $ENV{GLEW_DIR}/lib
        $ENV{GLEW_HOME}/lib
        /usr/lib64
        /usr/local/lib64
        /sw/lib64
        /opt/local/lib64
        DOC "The GLEW library")

ELSE()

    FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
        ${PROJECT_SOURCE_DIR}/3rdparty/glew-1.7.0/include
        $ENV{TMR_HOME}/../3rdparty/glew-1.7.0/include
        $ENV{GLEW_DIR}/include
        $ENV{GLEW_HOME}/include
        $ENV{PROGRAMFILES}/GLEW/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where GL/glew.h resides")

    FIND_LIBRARY(GLEW_LIBRARY
        NAMES GLEW glew glew32 glew32s
        PATHS
        ${PROJECT_SOURCE_DIR}/3rdparty/glew-1.7.0/lib
        $ENV{TMR_HOME}/../3rdparty/glew-1.7.0/lib
        $ENV{GLEW_DIR}/lib
        $ENV{GLEW_HOME}/lib
        /usr/lib
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        DOC "The GLEW library")

ENDIF()
    
IF(GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
    SET(GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ELSE()
    SET(GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
    MESSAGE(WARNING "Note: an envvar like GLEW_HOME assists this script to locate glew.")
ENDIF()

MARK_AS_ADVANCED( GLEW_FOUND )
