
set(target glow)
message(STATUS "lib ${target}")

set(HEADER_PATH "${GLOW_INCLUDE_DIR}/${target}")
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

configure_file(${HEADER_PATH}/meta.h.in meta.h)

set(HEADERS
    ${HEADER_PATH}/declspec.h
    
    ${HEADER_PATH}/FileAssociatedShader.h
    ${HEADER_PATH}/GpuQuery.h
    ${HEADER_PATH}/Object.h
    ${HEADER_PATH}/ArrayData.h
    ${HEADER_PATH}/Array.hpp
    ${HEADER_PATH}/Buffer.h
    ${HEADER_PATH}/Error.h
    ${HEADER_PATH}/Program.h
    ${HEADER_PATH}/Shader.h
    ${HEADER_PATH}/Texture.h
    ${HEADER_PATH}/VertexArrayObject.h
)

set(SOURCES

    ${SOURCE_PATH}/FileAssociatedShader.cpp
    ${SOURCE_PATH}/GpuQuery.cpp
    ${SOURCE_PATH}/Program.cpp
    ${SOURCE_PATH}/Shader.cpp

    ${SOURCE_PATH}/internal/Dummy.cpp
    
    ${SOURCE_PATH}/Object.cpp
    ${SOURCE_PATH}/Buffer.cpp
    ${SOURCE_PATH}/Error.cpp
    ${SOURCE_PATH}/Program.cpp
    ${SOURCE_PATH}/Shader.cpp
    ${SOURCE_PATH}/Texture.cpp
    ${SOURCE_PATH}/VertexArrayObject.cpp
)


# group source files

source_group_by_path(${HEADER_PATH} "\\\\.h$|\\\\.hpp$" 
    ${GLOW_HEADER_GROUP} ${HEADERS})
source_group_by_path(${SOURCE_PATH} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${GLOW_SOURCE_GROUP} ${SOURCES})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${GLOW_INCLUDE_DIR})

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
