cmake_minimum_required(VERSION 2.8)

set(target computeshader)
project(${target})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
	${GLOW_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${target} 
	main.cpp
)

target_link_libraries(${target} 
	${GLEW_LIBRARIES}
	glow
)

set_target_properties(${target}
    PROPERTIES
    FOLDER ${GLOW_EXAMPLES_FOLDER}
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#    LIBRARY DESTINATION ${INSTALL_SHARED}
#    ARCHIVE DESTINATION ${INSTALL_LIB}
)
