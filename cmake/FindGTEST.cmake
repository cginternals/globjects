
# GTEST_FOUND
# GTEST_INCLUDE_DIR
# GTEST_LIBRARY

find_path(GTEST_INCLUDE_DIR gtest/gtest.h
    $ENV{GTESTDIR}/include
    $ENV{GTEST_HOME}/include
    $ENV{PROGRAMFILES}/GTEST/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where gtest/gtest.h resides")

find_library(GTEST_LIBRARY
    NAMES gtest
    PATHS
    $ENV{GTESTDIR}/lib
    $ENV{GTEST_HOME}/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The GTEST library")
   
find_package_handle_standard_args(GTEST REQUIRED_VARS GTEST_INCLUDE_DIR GTEST_LIBRARY)
mark_as_advanced(GTEST_INCLUDE_DIR GTEST_LIBRARY)
