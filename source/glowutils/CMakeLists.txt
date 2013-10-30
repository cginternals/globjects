
# Target
set(target glowutils)
message(STATUS "lib ${target}")

# Libraries
set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    glow
)

# Definitions
add_definitions("-DGLOWUTILS_EXPORTS")

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# Sources
set(header_path "${GLOW_SOURCE_DIR}/include/glowutils")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${header_path}/glowutils.h

    ${header_path}/AbstractCoordinateProvider.h
    ${header_path}/AxisAlignedBoundingBox.h
    ${header_path}/AdaptiveGrid.h
    ${header_path}/AxonometricLookAt.h
    ${header_path}/CachedValue.h
    ${header_path}/CachedValue.hpp
    ${header_path}/Camera.h   
    ${header_path}/Icosahedron.h
    ${header_path}/MathMacros.h
    ${header_path}/NavigationMath.h
    ${header_path}/Plane3.h
    ${header_path}/ScreenAlignedQuad.h
    ${header_path}/SourceTemplate.h
    ${header_path}/TrackballNavigation.h
    ${header_path}/UniformGroup.h
    ${header_path}/UniformGroup.hpp
    ${header_path}/UnitCube.h
    ${header_path}/WorldInHandNavigation.h
)

set(sources
    ${source_path}/AdaptiveGrid.cpp
    ${source_path}/AxisAlignedBoundingBox.cpp
    ${source_path}/AxonometricLookAt.cpp
    ${source_path}/Camera.cpp
    ${source_path}/Icosahedron.cpp
    ${source_path}/NavigationMath.cpp
    ${source_path}/Plane3.cpp
    ${source_path}/ScreenAlignedQuad.cpp
    ${source_path}/SourceTemplate.cpp
    ${source_path}/TrackballNavigation.cpp
    ${source_path}/UniformGroup.cpp
    ${source_path}/UnitCube.cpp
    ${source_path}/WorldInHandNavigation.cpp
)

# Group source files
set(glowutils_header_group "Header Files (API)")
set(glowutils_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glowutils_header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glowutils_source_group} ${sources})

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()
target_link_libraries(${target} ${libs})
set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            "${GLOW_SOURCE_DIR}/include")

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)
