
set(target globjects)
message(STATUS "Lib ${target}")


# External libraries

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)


# Includes

include_directories(
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Libraries

set(libs
	${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARIES}
)


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-D${META_PROJECT_NAME_UPPER}_STATIC")
else()
    add_definitions("-DGLOBJECTS_EXPORTS")
endif()

if (OPTION_ERRORS_AS_EXCEPTION)
    add_definitions("-DGLOBJECTS_GL_ERROR_RAISE_EXCEPTION")
else()
    add_definitions("-D_HAS_EXCEPTIONS=0")
endif()

# for compatibility between glm 0.9.4 and 0.9.5
add_definitions("-DGLM_FORCE_RADIANS")


# Sources

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(sources
	${source_path}/AbstractState.cpp
	${source_path}/AbstractUniform.cpp
	${source_path}/Buffer.cpp
	${source_path}/Capability.cpp
	${source_path}/container_helpers.hpp
	${source_path}/DebugMessage.cpp
	${source_path}/Error.cpp
	${source_path}/FramebufferAttachment.cpp
	${source_path}/Framebuffer.cpp
	${source_path}/glbindinglogging.cpp
	${source_path}/glmlogging.cpp
	${source_path}/globjects.cpp
	
	${source_path}/base/AbstractFunctionCall.cpp
	${source_path}/base/AbstractStringSource.cpp
	${source_path}/base/baselogging.cpp
	${source_path}/base/Changeable.cpp
	${source_path}/base/ChangeListener.cpp
	${source_path}/base/CompositeStringSource.cpp
	${source_path}/base/ConsoleLogger.cpp
	${source_path}/base/File.cpp
	${source_path}/base/FileRegistry.cpp
	${source_path}/base/FileRegistry.h
	${source_path}/base/formatString.cpp
	${source_path}/base/HeapOnly.cpp
	${source_path}/base/LogMessageBuilder.cpp
	${source_path}/base/LogMessage.cpp
	${source_path}/base/Referenced.cpp
	${source_path}/base/StaticStringSource.cpp
	${source_path}/base/StringSourceDecorator.cpp
	${source_path}/base/StringTemplate.cpp

	${source_path}/implementations/AbstractBufferImplementation.cpp
	${source_path}/implementations/AbstractBufferImplementation.h

	${source_path}/implementations/AbstractObjectNameImplementation.h
	${source_path}/implementations/AbstractObjectNameImplementation.cpp
	${source_path}/implementations/ObjectNameImplementation_KHR_debug.h
	${source_path}/implementations/ObjectNameImplementation_KHR_debug.cpp
	${source_path}/implementations/ObjectNameImplementation_Legacy.h
	${source_path}/implementations/ObjectNameImplementation_Legacy.cpp

	${source_path}/implementations/BufferImplementation_DirectStateAccessARB.cpp
	${source_path}/implementations/BufferImplementation_DirectStateAccessARB.h
	${source_path}/implementations/BufferImplementation_DirectStateAccessEXT.cpp
	${source_path}/implementations/BufferImplementation_DirectStateAccessEXT.h
	${source_path}/implementations/BufferImplementation_Legacy.cpp
	${source_path}/implementations/BufferImplementation_Legacy.h

	${source_path}/implementations/AbstractDebugImplementation.cpp
	${source_path}/implementations/AbstractDebugImplementation.h
	${source_path}/implementations/DebugImplementation_DebugKHR.cpp
	${source_path}/implementations/DebugImplementation_DebugKHR.h
	${source_path}/implementations/DebugImplementation_Legacy.cpp
	${source_path}/implementations/DebugImplementation_Legacy.h

	${source_path}/implementations/AbstractFramebufferImplementation.cpp
	${source_path}/implementations/AbstractFramebufferImplementation.h
	${source_path}/implementations/FramebufferImplementation_DirectStateAccessARB.cpp
	${source_path}/implementations/FramebufferImplementation_DirectStateAccessARB.h
	${source_path}/implementations/FramebufferImplementation_DirectStateAccessEXT.cpp
	${source_path}/implementations/FramebufferImplementation_DirectStateAccessEXT.h
	${source_path}/implementations/FramebufferImplementation_Legacy.cpp
	${source_path}/implementations/FramebufferImplementation_Legacy.h

	${source_path}/implementations/AbstractProgramBinaryImplementation.cpp
	${source_path}/implementations/AbstractProgramBinaryImplementation.h
	${source_path}/implementations/ProgramBinaryImplementation_GetProgramBinaryARB.cpp
	${source_path}/implementations/ProgramBinaryImplementation_GetProgramBinaryARB.h
	${source_path}/implementations/ProgramBinaryImplementation_None.cpp
	${source_path}/implementations/ProgramBinaryImplementation_None.h

	${source_path}/implementations/AbstractShadingLanguageIncludeImplementation.cpp
	${source_path}/implementations/AbstractShadingLanguageIncludeImplementation.h
	${source_path}/implementations/ShadingLanguageIncludeImplementation_ARB.cpp
	${source_path}/implementations/ShadingLanguageIncludeImplementation_ARB.h
	${source_path}/implementations/ShadingLanguageIncludeImplementation_Fallback.cpp
	${source_path}/implementations/ShadingLanguageIncludeImplementation_Fallback.h

	${source_path}/implementations/AbstractUniformImplementation.cpp
	${source_path}/implementations/AbstractUniformImplementation.h
	${source_path}/implementations/UniformImplementation_Legacy.cpp
	${source_path}/implementations/UniformImplementation_Legacy.h
	${source_path}/implementations/UniformImplementation_SeparateShaderObjectsARB.cpp
	${source_path}/implementations/UniformImplementation_SeparateShaderObjectsARB.h
	
	${source_path}/implementations/AbstractVertexAttributeBindingImplementation.cpp
	${source_path}/implementations/AbstractVertexAttributeBindingImplementation.h
	${source_path}/implementations/VertexAttributeBindingImplementation_DirectStateAccessARB.cpp
	${source_path}/implementations/VertexAttributeBindingImplementation_DirectStateAccessARB.h
	${source_path}/implementations/VertexAttributeBindingImplementation_VertexAttribBindingARB.cpp
	${source_path}/implementations/VertexAttributeBindingImplementation_VertexAttribBindingARB.h
	${source_path}/implementations/VertexAttributeBindingImplementation_Legacy.cpp
	${source_path}/implementations/VertexAttributeBindingImplementation_Legacy.h

	${source_path}/IncludeProcessor.cpp
	${source_path}/IncludeProcessor.h
	${source_path}/LocationIdentity.cpp
	${source_path}/memory.cpp
	${source_path}/NamedString.cpp
	${source_path}/Object.cpp
	${source_path}/objectlogging.cpp
	${source_path}/ObjectVisitor.cpp
	${source_path}/pixelformat.cpp
	${source_path}/pixelformat.h
	${source_path}/ProgramBinary.cpp
	${source_path}/Program.cpp
	${source_path}/Query.cpp
	${source_path}/registry/ObjectRegistry.h
	${source_path}/registry/ExtensionRegistry.h
	${source_path}/registry/NamedStringRegistry.cpp
	${source_path}/registry/ImplementationRegistry.cpp
	${source_path}/registry/ExtensionRegistry.cpp
	${source_path}/registry/ObjectRegistry.cpp
	${source_path}/registry/NamedStringRegistry.h
	${source_path}/registry/ImplementationRegistry.h
	${source_path}/registry/Registry.cpp
	${source_path}/registry/Registry.h
	${source_path}/AttachedRenderbuffer.cpp
	${source_path}/Renderbuffer.cpp
	${source_path}/Resource.cpp
	${source_path}/Resource.h
	${source_path}/Sampler.cpp
	${source_path}/Shader.cpp
	${source_path}/State.cpp
	${source_path}/StateSetting.cpp
	${source_path}/Sync.cpp
	${source_path}/AttachedTexture.cpp
	${source_path}/Texture.cpp
	${source_path}/TransformFeedback.cpp
	${source_path}/UniformBlock.cpp
	${source_path}/VertexArray.cpp
	${source_path}/VertexAttributeBinding.cpp
)

set(api_includes
	${include_path}/AbstractState.h
	${include_path}/AbstractState.hpp
	${include_path}/AbstractUniform.h
	${include_path}/AbstractUniform.hpp
	${include_path}/Buffer.h
	${include_path}/Buffer.hpp
	${include_path}/Capability.h
	${include_path}/DebugMessage.h
	${include_path}/Error.h
	${include_path}/FramebufferAttachment.h
	${include_path}/Framebuffer.h
	${include_path}/glbindinglogging.h
	${include_path}/glmlogging.h
	${include_path}/globjects_api.h
	${include_path}/globjects.h
	${include_path}/globjects.hpp
	${include_path}/LocationIdentity.h
	${include_path}/logging.h
	${include_path}/memory.h
	${include_path}/NamedString.h
	${include_path}/Object.h
	${include_path}/objectlogging.h
	${include_path}/objectlogging.hpp
	${include_path}/ObjectVisitor.h
	${include_path}/ProgramBinary.h
	${include_path}/Program.h
	${include_path}/Program.hpp
	${include_path}/Query.h
	${include_path}/AttachedRenderbuffer.h
	${include_path}/Renderbuffer.h
	${include_path}/Sampler.h
	${include_path}/Shader.h
	${include_path}/State.h
	${include_path}/StateSetting.h
	${include_path}/StateSetting.hpp
	${include_path}/Sync.h
	${include_path}/AttachedTexture.h
	${include_path}/Texture.h
	${include_path}/TextureHandle.h
	${include_path}/TransformFeedback.h
	${include_path}/TransformFeedback.hpp
	${include_path}/UniformBlock.h
	${include_path}/Uniform.h
	${include_path}/Uniform.hpp
	${include_path}/VertexArray.h
	${include_path}/VertexAttributeBinding.h
	
	${include_path}/base/AbstractStringSource.h
	${include_path}/base/AbstractFunctionCall.h
	${include_path}/base/AbstractLogHandler.h
	${include_path}/base/baselogging.h
	${include_path}/base/baselogging.hpp
	${include_path}/base/CachedValue.h
	${include_path}/base/CachedValue.hpp
	${include_path}/base/Changeable.h
	${include_path}/base/ChangeListener.h
	${include_path}/base/CompositeStringSource.h
	${include_path}/base/ConsoleLogger.h
	${include_path}/base/container_helpers.hpp
	${include_path}/base/File.h
	${include_path}/base/formatString.h
	${include_path}/base/formatString.hpp
	${include_path}/base/FunctionCall.h
	${include_path}/base/FunctionCall.hpp
	${include_path}/base/HeapOnly.h
	${include_path}/base/LogMessageBuilder.h
	${include_path}/base/LogMessageBuilder.hpp
	${include_path}/base/LogMessage.h
	${include_path}/base/LogMessageLevel.h
	${include_path}/base/Referenced.h
	${include_path}/base/ref_ptr.h
	${include_path}/base/ref_ptr.hpp
	${include_path}/base/Singleton.h
	${include_path}/base/Singleton.hpp
	${include_path}/base/StaticStringSource.h
	${include_path}/base/StringSourceDecorator.h
	${include_path}/base/StringTemplate.h
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

# Build library

add_library(${target} ${api_includes} ${sources})

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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})


# Deployment

# Library
if(NOT OPTION_BUILD_STATIC)
    install(TARGETS ${target} COMPONENT runtime
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_SHARED})
    if(WIN32)
		install(FILES ${GLBINDING_BINARY_RELEASE} DESTINATION ${INSTALL_BIN}
			CONFIGURATIONS Release COMPONENT dev)
		install(FILES ${GLBINDING_BINARY_DEBUG} DESTINATION ${INSTALL_BIN}
			CONFIGURATIONS Debug COMPONENT dev)
    endif()

else()
    install(TARGETS ${target} COMPONENT dev
        ARCHIVE DESTINATION ${INSTALL_LIB})
endif()

if(WIN32 AND NOT OPTION_BUILD_STATIC) # on windows, provide self contained zip files (examples and dev, both contain the run-time)

    if(OPTION_BUILD_EXAMPLES)
        install(TARGETS ${target} COMPONENT examples
            RUNTIME DESTINATION ${INSTALL_BIN}
            LIBRARY DESTINATION ${INSTALL_BIN})
    endif()

    install(TARGETS ${target} COMPONENT dev
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_SHARED}
        ARCHIVE DESTINATION ${INSTALL_LIB})

endif()

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE} COMPONENT dev)
