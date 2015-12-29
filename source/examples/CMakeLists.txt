
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# Examples
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
