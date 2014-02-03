
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
if (NOT OPTION_BUILD_STATIC)
    add_definitions("-DGLOWUTILS_EXPORTS")
endif()

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
    ${header_path}/AbstractTransparencyAlgorithm.h
    ${header_path}/ABufferAlgorithm.h
    ${header_path}/AxisAlignedBoundingBox.h
    ${header_path}/AdaptiveGrid.h
    ${header_path}/AutoTimer.h
    ${header_path}/AxonometricLookAt.h
    ${header_path}/CachedValue.h
    ${header_path}/CachedValue.hpp
    ${header_path}/Camera.h
    ${header_path}/CameraPath.h
    ${header_path}/CameraPathPlayer.h
    ${header_path}/CameraPathRecorder.h
    ${header_path}/File.h
    ${header_path}/FlightNavigation.h
    ${header_path}/GlBlendAlgorithm.h
    ${header_path}/global.h
    ${header_path}/HybridAlgorithm.h
    ${header_path}/Icosahedron.h
    ${header_path}/Interpolation.h
    ${header_path}/navigationmath.h
    ${header_path}/Plane3.h
    ${header_path}/RawFile.h
    ${header_path}/RawFile.hpp
    ${header_path}/screen.h
    ${header_path}/ScreenAlignedQuad.h
    ${header_path}/StackedState.h
    ${header_path}/StringSourceDecorator.h
    ${header_path}/StringTemplate.h
    ${header_path}/Timer.h
    ${header_path}/TrackballNavigation.h
    ${header_path}/UniformGroup.h
    ${header_path}/UniformGroup.hpp
    ${header_path}/UnitCube.h
    ${header_path}/WeightedAverageAlgorithm.h
    ${header_path}/WorldInHandNavigation.h
)

set(sources
    ${source_path}/AbstractCoordinateProvider.cpp
    ${source_path}/AbstractTransparencyAlgorithm.cpp
    ${source_path}/ABufferAlgorithm.cpp
    ${source_path}/AdaptiveGrid.cpp
    ${source_path}/AutoTimer.cpp
    ${source_path}/AxisAlignedBoundingBox.cpp
    ${source_path}/AxonometricLookAt.cpp
    ${source_path}/Camera.cpp
    ${source_path}/CameraPath.cpp
    ${source_path}/CameraPathPlayer.cpp
    ${source_path}/CameraPathRecorder.cpp
    ${source_path}/File.cpp
    ${source_path}/FileRegistry.h
    ${source_path}/FileRegistry.cpp
    ${source_path}/FlightNavigation.cpp
    ${source_path}/GlBlendAlgorithm.cpp
    ${source_path}/global.cpp
    ${source_path}/HybridAlgorithm.cpp
    ${source_path}/Icosahedron.cpp
    ${source_path}/navigationmath.cpp
    ${source_path}/Plane3.cpp
    ${source_path}/screen.cpp
    ${source_path}/ScreenAlignedQuad.cpp
    ${source_path}/StackedState.cpp
    ${source_path}/StringSourceDecorator.cpp
    ${source_path}/StringTemplate.cpp
    ${source_path}/Timer.cpp
    ${source_path}/TrackballNavigation.cpp
    ${source_path}/UniformGroup.cpp
    ${source_path}/UnitCube.cpp
    ${source_path}/WeightedAverageAlgorithm.cpp
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
