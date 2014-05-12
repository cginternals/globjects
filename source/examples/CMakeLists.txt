
if(OPTION_BUILD_EXAMPLES AND OPTION_BUILD_GLOW_UTILS AND OPTION_BUILD_GLOW_WINDOW)
	set(EXAMPLE_COMMON examplecommon)

        # for compatibility between glm 0.9.4 and 0.9.5
        add_definitions("-DGLM_FORCE_RADIANS")
	
	find_package(OpenGL REQUIRED)
	find_package(GLM REQUIRED)
	find_package(GLFW REQUIRED)
	find_package(GLBinding REQUIRED)
	
	set(GLOW_EXAMPLE_DEPENDENCY_INCLUDES
		${GLM_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR}
		${GLBINDING_INCLUDE_DIR}
	)

	set(GLOW_EXAMPLE_INCLUDES
		${CMAKE_SOURCE_DIR}/source/glowbase/include
		${CMAKE_SOURCE_DIR}/source/glow/include
		${CMAKE_SOURCE_DIR}/source/glowutils/include
		${CMAKE_SOURCE_DIR}/source/glowwindow/include
	)
	
	set(GLOW_EXAMPLES_LIBRARIES
		${OPENGL_LIBRARIES}
		${GLBINDING_LIBRARY}
		${GLFW_LIBRARY}
		glowbase
		glow
		glowutils
		glowwindow
	)
	
	add_subdirectory("common")
	include_directories("common")
	
	set(GLOW_EXAMPLES_LIBRARIES
		${GLOW_EXAMPLES_LIBRARIES}
		${EXAMPLE_COMMON}
	)

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
elseif(OPTION_BUILD_EXAMPLES)
	if (OPTION_BUILD_GLOW_UTILS)
		message(STATUS "Examples can't be build because glowwindow is not build")
	elseif(OPTION_BUILD_GLOW_WINDOW)
		message(STATUS "Examples can't be build because glowutils is not build")
	else()
		message(STATUS "Examples can't be build because glowutils and glowwindow is not build")
	endif()
endif()
