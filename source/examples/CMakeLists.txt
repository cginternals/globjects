
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()


#
# Examples
#

add_subdirectory("commandlineoutput")
add_subdirectory("computeshader")
add_subdirectory("programpipelines")
add_subdirectory("shaderincludes")
add_subdirectory("sparsetexture")
add_subdirectory("ssbo")
add_subdirectory("states")
add_subdirectory("texture")
add_subdirectory("tessellation")
add_subdirectory("transformfeedback")

add_subdirectory("qtexample")
add_subdirectory("qtexample-es")
