
# this must be explicitly set.. not quitly if available.

if(OPTION_REQUIRE_TESTS)
	find_package(GMOCK REQUIRED)
	find_package(GTEST REQUIRED)
else()
	find_package(GMOCK QUIET)
	find_package(GTEST QUIET)
endif()

if(GMOCK_FOUND AND GTEST_FOUND)

	set(GLOW_TESTS_FOLDER "Tests")

	include_directories(
		${GMOCK_INCLUDE_DIR}
		${GTEST_INCLUDE_DIR}
	)

	unset(sources)
	unset(api_includes)

	add_subdirectory("glow-test")
	add_subdirectory("glowutils-test")
	add_subdirectory("glowwindow-test")

	set(GLOW_TESTS "tests")
	add_custom_target(${GLOW_TESTS})

	add_dependencies(${GLOW_TESTS} glow-test glowutils-test glowwindow-test)
	add_custom_command(TARGET ${GLOW_TESTS} POST_BUILD 
		COMMAND cmake -E echo
		COMMAND echo run $<TARGET_FILE:glow-test>:
		COMMAND $<TARGET_FILE:glow-test>)
	add_custom_command(TARGET ${GLOW_TESTS} POST_BUILD 
		COMMAND cmake -E echo
		COMMAND echo run $<TARGET_FILE:glowutils-test>:
		COMMAND $<TARGET_FILE:glowutils-test>)
	add_custom_command(TARGET ${GLOW_TESTS} POST_BUILD 
		COMMAND cmake -E echo
		COMMAND echo run $<TARGET_FILE:glowwindow-test>:
		COMMAND $<TARGET_FILE:glowwindow-test>)

endif()
