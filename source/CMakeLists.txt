add_subdirectory(glow)
add_subdirectory(glowutils)

if(OPTION_BUILD_EXAMPLES)
	add_subdirectory(glowwindow)
endif()