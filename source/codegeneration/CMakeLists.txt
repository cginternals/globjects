
set(GENERATED_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

if (OPTION_GENERATE_GL_INFO)

	set(GENERATED_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}")

	set(GL_SPEC "${CMAKE_CURRENT_SOURCE_DIR}/gl.xml")
	set(GENERATED_CONSTANTS "${GENERATED_HEADER_PATH}/glow/gl_constants.h")
	set(GENERATED_EXTENSIONS "${GENERATED_HEADER_PATH}/glow/gl_extensions.h")
	set(GENERATED_EXTENSION_INFO "${GENERATED_HEADER_PATH}/glow/gl_extension_info.h")
	
	set(CONSTANTS_GENERATOR "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_constants.py")
	set(EXTENSIONS_GENERATOR "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_extensions.py")

	find_program(PYTHON "python")

	if (PYTHON)
		message(STATUS "Generate constants")
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E make_directory glow
			WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" 
		)
		execute_process(
			COMMAND ${PYTHON} ${CONSTANTS_GENERATOR} -i ${GL_SPEC} -o ${GENERATED_CONSTANTS}
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" 
		)
		message(STATUS "Generate extensions")
		execute_process(
			COMMAND ${PYTHON} ${EXTENSIONS_GENERATOR} -i ${GL_SPEC} -e ${GENERATED_EXTENSIONS} -o ${GENERATED_EXTENSION_INFO}
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" 
		)
	else()
		message(SEND_ERROR "Could not find python")
	endif()

endif()

# Install generated files
install(FILES "${GENERATED_HEADER_PATH}/glow/gl_extensions.h"  DESTINATION "${INSTALL_INCLUDE}/glow")

set_property(GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY ${GENERATED_HEADER_PATH})
