
set(target ssbo)
message(STATUS "Example ${target}")

# External libraries

find_package(GLM REQUIRED)
find_package(GLFW REQUIRED)
find_package(glbinding REQUIRED)

# Includes

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/globjects/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Libraries

set(libs
    ${GLFW_LIBRARY}
    ${GLBINDING_LIBRARIES}
    globjects
)

# Sources

set(sources
    main.cpp
    ScreenAlignedQuad.h
    ScreenAlignedQuad.cpp
)

# Target definitions

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

# Build executable

add_executable(${target} ${sources})

target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

# Deployment

install(TARGETS ${target} COMPONENT examples
    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
