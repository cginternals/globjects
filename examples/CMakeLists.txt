
set(GLOW_EXAMPLES_FOLDER "Examples")

if(WIN32)
	install(FILES ${GLEW_INCLUDE_DIR}/../bin/glew32.dll DESTINATION ${INSTALL_EXAMPLES})
endif()

install(DIRECTORY ${GLOW_SOURCE_DIR}/data DESTINATION ${INSTALL_EXAMPLES})

unset(sources)
unset(api_includes)

include_directories(
	${GLFW_INCLUDE_DIR})

set(EXAMPLE_COMMON examplecommon)
add_subdirectory("common")
include_directories("common")

add_subdirectory("camerapath")
add_subdirectory("commandlineoutput")
add_subdirectory("computeshader")
add_subdirectory("gpu-particles")
add_subdirectory("multiple-contexts")
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
