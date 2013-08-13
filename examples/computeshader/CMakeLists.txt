CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(ComputeShaderDemo)

SET(target computeshaderdemo)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(FREEGLUT REQUIRED)

# Include directories
INCLUDE_DIRECTORIES(
	.
	..
	${OPENGL_INCLUDE_DIR}
	${GLM_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${FREEGLUT_INCLUDE_PATH}
	${GLOW_INCLUDE_DIR}
)

# Libraries
SET(libs
	${GLEW_LIBRARIES}
	${FREEGLUT_LIBRARY}
	glow
)

# Sources
SET(sources
	main.cpp
	CSWindow.cpp
	../GlutWindow.cpp
)

SET(headers
	CSWindow.h
	../GlutWindow.h
)

ADD_EXECUTABLE(${target} ${sources} ${headers})

TARGET_LINK_LIBRARIES(${target} ${libs})

# Deployment
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_EXAMPLES}
#    LIBRARY DESTINATION ${INSTALL_SHARED}
#    ARCHIVE DESTINATION ${INSTALL_LIB}
)
