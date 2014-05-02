
if(OPTION_BUILD_EXAMPLES)
	if(WIN32)
		install(FILES ${GLEW_INCLUDE_DIR}/../bin/glew32.dll DESTINATION ${INSTALL_BIN})
	endif()

	set(EXAMPLE_COMMON examplecommon)

        # for compatibility between glm 0.9.4 and 0.9.5
        add_definitions("-DGLM_FORCE_RADIANS")
	
	get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

	set(GLOW_EXAMPLE_INCLUDES
		${GENERATED_HEADER_PATH}
		${CMAKE_SOURCE_DIR}/source/glowbase/include
		${CMAKE_SOURCE_DIR}/source/glow/include
		${CMAKE_SOURCE_DIR}/source/glowutils/include
		${CMAKE_SOURCE_DIR}/source/glowwindow/include
	)
	
	set(GLOW_EXAMPLES_LIBRARIES
		glowbase
		glow
		glowutils
		glowwindow
		${EXAMPLE_COMMON}
	)
	
	add_subdirectory("common")
	include_directories("common")

	add_subdirectory("bindless-textures")
	add_subdirectory("camerapath")
	add_subdirectory("commandlineoutput")
	add_subdirectory("computeshader")
	add_subdirectory("gpu-particles")
	add_subdirectory("glraw-texture")
	add_subdirectory("multiple-contexts")
	add_subdirectory("mipmap-filtering")
	add_subdirectory("navigations")
	#add_subdirectory("deferred-lighting")
	add_subdirectory("post-processing")
	add_subdirectory("states")
	add_subdirectory("simple-texture")
	add_subdirectory("simple-transformfeedback")
	add_subdirectory("single-context")
	add_subdirectory("shaderincludes")
	add_subdirectory("ssbo")
	add_subdirectory("tessellation")
	add_subdirectory("vertexarrayattributes")
	add_subdirectory("wikiexample")
	add_subdirectory("transparency")
	add_subdirectory("sparse-texture")
endif()
