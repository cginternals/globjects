
if(OPTION_BUILD_EXAMPLES)
	set(EXAMPLE_COMMON common)

        # for compatibility between glm 0.9.4 and 0.9.5
        add_definitions("-DGLM_FORCE_RADIANS")
	
	find_package(GLM REQUIRED)
	find_package(GLFW REQUIRED)
	find_package(GLBinding REQUIRED)
	
	set(GLOBJECTS_EXAMPLE_DEPENDENCY_INCLUDES
		${GLM_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR}
		${GLBINDING_INCLUDES}
	)

	set(GLOBJECTS_EXAMPLE_INCLUDES
		${CMAKE_SOURCE_DIR}/source/globjects/include
		${CMAKE_SOURCE_DIR}/source/examples
	)
	
	set(GLOBJECTS_EXAMPLES_LIBRARIES
		${GLBINDING_LIBRARIES}
		${GLFW_LIBRARY}
		globjects
	)
	
	add_subdirectory("common")
	include_directories("common")
	
	set(GLOBJECTS_EXAMPLES_LIBRARIES
		${GLOBJECTS_EXAMPLES_LIBRARIES}
		${EXAMPLE_COMMON}
	)

	add_subdirectory("bindless-textures")
	add_subdirectory("commandlineoutput")
	add_subdirectory("computeshader")
	add_subdirectory("gbuffers")
	add_subdirectory("gpu-particles")
	add_subdirectory("glraw-texture")
	add_subdirectory("multiple-contexts")
	add_subdirectory("states")
	add_subdirectory("texture")
	add_subdirectory("transformfeedback")
	add_subdirectory("shaderincludes")
	add_subdirectory("ssbo")
	add_subdirectory("tessellation")
	add_subdirectory("wikiexample")
	add_subdirectory("sparse-texture")
	
	add_subdirectory("osgexample")
	add_subdirectory("qtexample")
endif()
