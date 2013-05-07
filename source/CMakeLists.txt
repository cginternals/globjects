
set(target glow)
message(STATUS "lib ${target}")

set(HEADER_PATH "${GLOW_INCLUDE_DIR}/${target}")
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

configure_file(${SOURCE_PATH}/meta.h.in meta.h)

set(HEADERS

    ${HEADER_PATH}/Dummy.h
)

set(SOURCES

    ${SOURCE_PATH}/declspec.h

    ${SOURCE_PATH}/Dummy.cpp

    ${SOURCE_PATH}/internal/Dummy.cpp
)


# group source files

source_group_by_path(${HEADER_PATH} "\\\\.h$|\\\\.hpp$" 
    ${GLOW_HEADER_GROUP} ${HEADERS})
source_group_by_path(${SOURCE_PATH} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${GLOW_SOURCE_GROUP} ${SOURCES})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})

if(OPTION_BUILD_STATIC)
    add_library(${target} ${HEADERS} ${SOURCES})
else()
    add_library(${target} SHARED ${HEADERS} ${SOURCES})
endif()

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)


set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")

add_definitions("-DGLOW_EXPORTS")
