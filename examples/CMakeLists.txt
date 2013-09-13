
set(GLOW_EXAMPLES_FOLDER "Examples")

if(WIN32)
	install(FILES ${GLEW_INCLUDE_DIR}/../bin/glew32.dll DESTINATION ${INSTALL_EXAMPLES})
endif()

install(DIRECTORY ${GLOW_SOURCE_DIR}/data DESTINATION ${INSTALL_EXAMPLES})

unset(sources)
unset(api_includes)

add_subdirectory("computeshader")
add_subdirectory("multiple-contexts")
add_subdirectory("deferred-lighting")
add_subdirectory("simple-texture")
add_subdirectory("simple-transformfeedback")
add_subdirectory("single-context")

#add_subdirectory("ssbo")
    