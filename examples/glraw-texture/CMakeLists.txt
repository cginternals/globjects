
set(target glraw-texture)
message(STATUS "example ${target}")

add_executable(${target} 
    main.cpp

    # copied from glraw ;)
	RawFile.h
	RawFile.cpp
)

target_link_libraries(${target} 
    ${GLEW_LIBRARIES}
    glow
    glowutils
    glowwindow
)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${GLOW_EXAMPLES_FOLDER}
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#    LIBRARY DESTINATION ${INSTALL_SHARED}
#    ARCHIVE DESTINATION ${INSTALL_LIB}
)
