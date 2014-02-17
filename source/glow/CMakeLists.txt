
# Target
set(target glow)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)

#
# Includes
#

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_BINARY_DIR}/source/pre-build/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLOW_STATIC")
else()
    add_definitions("-DGLOW_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/glow.h

    ${include_path}/AbstractLogHandler.h
    ${include_path}/AbstractUniform.h
    ${include_path}/AbstractState.h
    ${include_path}/AbstractState.hpp
    ${include_path}/AbstractStringSource.h
    ${include_path}/Buffer.h
    ${include_path}/Buffer.hpp
    ${include_path}/Capability.h
    ${include_path}/Changeable.h
    ${include_path}/ChangeListener.h
    ${include_path}/CompositeStringSource.h
    ${include_path}/ConsoleLogger.h
    ${include_path}/constants.h
    ${include_path}/DebugInfo.h
    ${include_path}/DebugMessage.h
    ${include_path}/debugmessageoutput.h
    ${include_path}/Error.h
    ${include_path}/Extension.h
    ${include_path}/FrameBufferAttachment.h
    ${include_path}/FrameBufferObject.h
    ${include_path}/formatString.h
    ${include_path}/formatString.hpp
    ${include_path}/FunctionCall.h
    ${include_path}/FunctionCall.hpp
    ${include_path}/global.h
    ${include_path}/global.hpp
    ${include_path}/logging.h
    ${include_path}/logging.hpp
    ${include_path}/LogMessage.h
    ${include_path}/LogMessageBuilder.h
    ${include_path}/LogMessageBuilder.hpp
    ${include_path}/memory.h
    ${include_path}/Object.h
    ${include_path}/ObjectRegistry.h
    ${include_path}/ObjectVisitor.h
    ${include_path}/Program.h
    ${include_path}/Program.hpp
    ${include_path}/ProgramBinary.h
    ${include_path}/Query.h
    ${include_path}/ref_ptr.h
    ${include_path}/ref_ptr.hpp
    ${include_path}/Referenced.h
    ${include_path}/RenderBufferAttachment.h
    ${include_path}/RenderBufferObject.h
    ${include_path}/Sampler.h
    ${include_path}/Shader.h
    ${include_path}/State.h
    ${include_path}/StateSetting.h
    ${include_path}/StateSetting.hpp
    ${include_path}/StaticStringSource.h
    ${include_path}/Texture.h
    ${include_path}/TextureAttachment.h
    ${include_path}/TextureHandle.h
    ${include_path}/TransformFeedback.h
    ${include_path}/TransformFeedback.hpp
    ${include_path}/Uniform.h
    ${include_path}/Uniform.hpp
    ${include_path}/UniformSetter.h
    ${include_path}/Version.h
    ${include_path}/VertexArrayObject.h
    ${include_path}/VertexAttributeBinding.h
)

set(sources
    ${source_path}/AbstractUniform.cpp
    ${source_path}/AbstractState.cpp
    ${source_path}/AbstractStringSource.cpp
    ${source_path}/Buffer.cpp
    ${source_path}/Capability.cpp
    ${source_path}/Changeable.cpp
    ${source_path}/ChangeListener.cpp
    ${source_path}/CompositeStringSource.cpp
    ${source_path}/ConsoleLogger.cpp
    ${source_path}/constants.cpp
    ${source_path}/contextid.h
    ${source_path}/contextid.cpp
    ${source_path}/DebugInfo.cpp
    ${source_path}/DebugMessage.cpp
    ${source_path}/DebugMessageCallback.h
    ${source_path}/DebugMessageCallback.cpp
    ${source_path}/debugmessageoutput.cpp
    ${source_path}/Error.cpp
    ${source_path}/Extension.cpp
    ${source_path}/FrameBufferAttachment.cpp
    ${source_path}/FrameBufferObject.cpp
    ${source_path}/formatString.cpp
    ${source_path}/global.cpp
    ${source_path}/IncludeProcessor.h
    ${source_path}/IncludeProcessor.cpp
    ${source_path}/logging.cpp
    ${source_path}/LogMessage.cpp
    ${source_path}/LogMessageBuilder.cpp
    ${source_path}/memory.cpp
    ${source_path}/NamedStrings.cpp
    ${source_path}/NamedStrings.h
    ${source_path}/Object.cpp
    ${source_path}/ObjectRegistry.cpp
    ${source_path}/ObjectVisitor.cpp
    ${source_path}/Program.cpp
    ${source_path}/ProgramBinary.cpp
    ${source_path}/Query.cpp
    ${source_path}/Referenced.cpp
    ${source_path}/RenderBufferAttachment.cpp
    ${source_path}/RenderBufferObject.cpp
    ${source_path}/Sampler.cpp
    ${source_path}/Shader.cpp
    ${source_path}/State.cpp
    ${source_path}/StateSetting.cpp
    ${source_path}/StaticStringSource.cpp
    ${source_path}/Texture.cpp
    ${source_path}/TextureAttachment.cpp
    ${source_path}/TransformFeedback.cpp
    ${source_path}/UniformSetter.cpp
    ${source_path}/Version.cpp
    ${source_path}/VertexArrayObject.cpp
    ${source_path}/VertexAttributeBinding.cpp
    ${source_path}/VertexAttributeBindingImplementation.h
    ${source_path}/VertexAttributeBindingImplementation.cpp
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
