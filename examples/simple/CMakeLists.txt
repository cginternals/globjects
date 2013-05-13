CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(simpleexample)

SET(target simpleexample)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)

INCLUDE_DIRECTORIES("include" ${GLOW_INCLUDE_DIR})

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source")

SET(HEADERS
	include/Widget.h
)

SET(SOURCES
	source/main.cpp
	source/Widget.cpp
)

source_group_by_path(${HEADER_PATH} "\\\\.h$|\\\\.hpp$" 
    "examples h" ${HEADERS})
source_group_by_path(${SOURCE_PATH} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    "examples src" ${SOURCES})
    


ADD_EXECUTABLE(${target} ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES(${target} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glow)
QT5_USE_MODULES(${target} Widgets OpenGL)

ADD_CUSTOM_TARGET(
	CopyResources ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data"
)

ADD_CUSTOM_COMMAND(
	TARGET CopyResources
	#OUTPUT "copy resources"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/data"
)

