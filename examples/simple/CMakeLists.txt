CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(simpleexample)

SET(target simpleexample)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(FREEGLUT REQUIRED)

INCLUDE_DIRECTORIES("include" ${GLOW_INCLUDE_DIR})

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source")

SET(HEADERS
	include/Window.h
)

SET(SOURCES
	source/main.cpp
	source/Window.cpp
)

source_group_by_path(${HEADER_PATH} "\\\\.h$|\\\\.hpp$" 
    "examples h" ${HEADERS})
source_group_by_path(${SOURCE_PATH} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    "examples src" ${SOURCES})

ADD_EXECUTABLE(${target} ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(${target} ${GLEW_LIBRARIES} ${FREEGLUT_LIBRARY} glow)

ADD_CUSTOM_TARGET(
	CopyResources ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data"
)

ADD_CUSTOM_COMMAND(
	TARGET CopyResources
	COMMAND ${CMAKE_COMMAND} -E echo ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/data"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/data"
)
