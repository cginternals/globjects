cmake_minimum_required(VERSION 2.8)

# Project
project(simpleexample)
set(target simpleexample)

# Find externals
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(FREEGLUT REQUIRED)

# Include directories
include_directories(
	${GLOW_INCLUDE_DIR}
	include
)

# Libraries
set(libs
	${GLEW_LIBRARIES}
	${FREEGLUT_LIBRARY}
	glow
)

# Definitions
#add_definitions("-D...")

# Sources
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers
	${header_path}/Window.h
)
set(sources
	${source_path}/main.cpp
	${source_path}/Window.cpp
)

# Group source files
set(header_group "Example Header Files")
set(source_group "Example Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

# Build executable
add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} ${libs})

# Copy data
# [TODO] REMOVE!
add_custom_target(
	CopyResources ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data"
)

add_custom_command(
	TARGET CopyResources
	COMMAND ${CMAKE_COMMAND} -E echo ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/data"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/data"
)
