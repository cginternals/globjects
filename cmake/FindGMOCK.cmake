
# GMOCK_FOUND
# GMOCK_INCLUDE_DIR
# GMOCK_LIBRARY

find_path(GMOCK_INCLUDE_DIR gmock/gmock.h
    $ENV{GMOCKDIR}/include
    $ENV{GMOCK_HOME}/include
    $ENV{PROGRAMFILES}/GMOCK/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where GMOCK/GMOCK.h resides")

find_library(GMOCK_LIBRARY
    NAMES gmock
    PATHS
    $ENV{GMOCKDIR}/lib
    $ENV{GMOCK_HOME}/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The GMOCK library")
   
find_package_handle_standard_args(GMOCK REQUIRED_VARS GMOCK_INCLUDE_DIR GMOCK_LIBRARY)
mark_as_advanced(GMOCK_INCLUDE_DIR GMOCK_LIBRARY)
