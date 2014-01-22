
# Target
set(target glow)
message(STATUS "lib ${target}")

# Libraries
set(libs
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
)

# Definitions
if (NOT OPTION_BUILD_STATIC)
    add_definitions("-DGLOW_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
	add_definitions("-DGLOW_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# Sources
set(header_path "${GLOW_SOURCE_DIR}/include/glow")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${header_path}/glow.h

    ${header_path}/AbstractLogHandler.h
    ${header_path}/AbstractUniform.h
    ${header_path}/AbstractArray.h
    ${header_path}/Array.h
    ${header_path}/Array.hpp
    ${header_path}/Buffer.h
    ${header_path}/Capability.h
    ${header_path}/Changeable.h
    ${header_path}/ChangeListener.h
    ${header_path}/CompositeStringSource.h
    ${header_path}/ConsoleLogger.h
    ${header_path}/constants.h
    ${header_path}/DebugInfo.h
    ${header_path}/DebugMessage.h
    ${header_path}/debugmessageoutput.h
    ${header_path}/Error.h
    ${header_path}/Extension.h
    ${header_path}/FrameBufferAttachment.h
    ${header_path}/FrameBufferObject.h
    ${header_path}/formatString.h
    ${header_path}/formatString.hpp
    ${header_path}/global.h
    ${header_path}/global.hpp
    ${header_path}/logging.h
    ${header_path}/logging.hpp
    ${header_path}/LogMessage.h
    ${header_path}/LogMessageBuilder.h
    ${header_path}/LogMessageBuilder.hpp
    ${header_path}/memory.h
    ${header_path}/Object.h
    ${header_path}/ObjectRegistry.h
    ${header_path}/ObjectVisitor.h
    ${header_path}/Program.h
    ${header_path}/Program.hpp
    ${header_path}/Query.h
    ${header_path}/ref_ptr.h
    ${header_path}/ref_ptr.hpp
    ${header_path}/Referenced.h
    ${header_path}/RenderBufferAttachment.h
    ${header_path}/RenderBufferObject.h
    ${header_path}/Sampler.h
    ${header_path}/Shader.h
    ${header_path}/State.h
    ${header_path}/String.h
    ${header_path}/StringSource.h
    ${header_path}/Texture.h
    ${header_path}/TextureAttachment.h
    ${header_path}/TextureHandle.h
    ${header_path}/TransformFeedback.h
    ${header_path}/Uniform.h
    ${header_path}/Uniform.hpp
    ${header_path}/UniformSetter.h
    ${header_path}/Version.h
    ${header_path}/VertexArrayObject.h
    ${header_path}/VertexAttributeBinding.h
)

set(sources
    ${source_path}/AbstractUniform.cpp
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
    ${source_path}/FeatureRegistry.cpp
    ${source_path}/FeatureRegistry.h
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
    ${source_path}/Query.cpp
    ${source_path}/Referenced.cpp
    ${source_path}/RenderBufferAttachment.cpp
    ${source_path}/RenderBufferObject.cpp
    ${source_path}/Sampler.cpp
    ${source_path}/Shader.cpp
    ${source_path}/State.cpp
    ${source_path}/String.cpp
    ${source_path}/StringSource.cpp
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
set(glow_header_group "Header Files (API)")
set(glow_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glow_header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glow_source_group} ${sources})

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
