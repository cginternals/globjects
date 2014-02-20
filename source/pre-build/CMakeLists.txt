
set(GL_SPEC "${CMAKE_CURRENT_SOURCE_DIR}/gl.xml")
set(GENERATED_CONSTANTS "${CMAKE_CURRENT_BINARY_DIR}/gl_constants.h")
set(GENERATED_EXTENSIONS "${CMAKE_CURRENT_BINARY_DIR}/gl_extensions.h")
set(GENERATED_EXTENSION_INFO "${CMAKE_CURRENT_BINARY_DIR}/gl_extension_info.h")

find_program(PYTHON "python")

if (PYTHON)
	message(STATUS "Generate constants")
	execute_process(
		COMMAND ${PYTHON} scripts/generate_constants.py -i ${GL_SPEC} -o ${GENERATED_CONSTANTS}
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" 
	)
	message(STATUS "Generate extensions")
	execute_process(
		COMMAND ${PYTHON} scripts/generate_extensions.py -i ${GL_SPEC} -e ${GENERATED_EXTENSIONS} -o ${GENERATED_EXTENSION_INFO}
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" 
	)
	
	set(PREBUILD_PATH "${CMAKE_CURRENT_BINARY_DIR}")
else()
	message(SEND_ERROR "Could not find python")
endif()
