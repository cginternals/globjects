
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif ()

if(WIN32)
	install(FILES ${GLBINDING_BINARY_RELEASE} DESTINATION ${INSTALL_EXAMPLES}
		CONFIGURATIONS Release COMPONENT examples)
	install(FILES ${GLBINDING_BINARY_DEBUG} DESTINATION ${INSTALL_EXAMPLES}
		CONFIGURATIONS Debug COMPONENT examples)
endif()

# add_subdirectory("bindless-textures")
add_subdirectory("commandlineoutput")
add_subdirectory("computeshader")
add_subdirectory("states")
add_subdirectory("texture")
add_subdirectory("transformfeedback")
add_subdirectory("shaderincludes")
add_subdirectory("ssbo")
add_subdirectory("tessellation")
add_subdirectory("wikiexample")
add_subdirectory("sparse-texture")

add_subdirectory("qt-example")
