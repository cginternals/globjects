
set(target glowwindow-test)
message(STATUS "test ${target}")

add_executable(${target} 
    main.cpp
)

target_link_libraries(${target} 
    ${GLEW_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    glowwindow
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${GLOW_TESTS_FOLDER}
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")
