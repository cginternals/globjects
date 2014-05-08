
# Target
set(target glowutils)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLBinding REQUIRED)

#
# Includes
#

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glowbase/include
    ${CMAKE_SOURCE_DIR}/source/glow/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARY}
    glowbase
    glow
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOWUTILS_STATIC")
else()
    add_definitions("-DGLOWUTILS_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/AbstractCoordinateProvider.cpp
	${source_path}/AdaptiveGrid.cpp
	${source_path}/AutoTimer.cpp
	${source_path}/AxisAlignedBoundingBox.cpp
	${source_path}/AxonometricLookAt.cpp
	${source_path}/Camera.cpp
	${source_path}/CameraPath.cpp
	${source_path}/CameraPathPlayer.cpp
	${source_path}/CameraPathRecorder.cpp
	${source_path}/dirent_msvc.h
	${source_path}/File.cpp
	${source_path}/FileRegistry.cpp
	${source_path}/FileRegistry.h
	${source_path}/FlightNavigation.cpp
	${source_path}/glowutils.cpp
	${source_path}/Icosahedron.cpp
	${source_path}/math/BezierCurve.cpp
	${source_path}/navigationmath.cpp
	${source_path}/Plane3.cpp
	${source_path}/ScreenAlignedQuad.cpp
	${source_path}/screen.cpp
	${source_path}/StackedState.cpp
	${source_path}/StringSourceDecorator.cpp
	${source_path}/StringTemplate.cpp
	${source_path}/Timer.cpp
	${source_path}/TrackballNavigation.cpp
	${source_path}/UniformGroup.cpp
	${source_path}/UnitCube.cpp
	${source_path}/VertexDrawable.cpp
	${source_path}/WorldInHandNavigation.cpp
)

set(api_includes
	${include_path}/AbstractCoordinateProvider.h
	${include_path}/AdaptiveGrid.h
	${include_path}/AutoTimer.h
	${include_path}/AxisAlignedBoundingBox.h
	${include_path}/AxonometricLookAt.h
	${include_path}/Camera.h
	${include_path}/CameraPath.h
	${include_path}/CameraPathPlayer.h
	${include_path}/CameraPathRecorder.h
	${include_path}/File.h
	${include_path}/FlightNavigation.h
	${include_path}/glowutils_api.h
	${include_path}/glowutils.h
	${include_path}/Icosahedron.h
	${include_path}/Interpolation.h
	${include_path}/math/BezierCurve.h
	${include_path}/navigationmath.h
	${include_path}/Plane3.h
	${include_path}/ScreenAlignedQuad.h
	${include_path}/screen.h
	${include_path}/StackedState.h
	${include_path}/StringSourceDecorator.h
	${include_path}/StringTemplate.h
	${include_path}/Timer.h
	${include_path}/TrackballNavigation.h
	${include_path}/UniformGroup.h
	${include_path}/UniformGroup.hpp
	${include_path}/UnitCube.h
	${include_path}/VertexDrawable.h
	${include_path}/WorldInHandNavigation.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()
target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
