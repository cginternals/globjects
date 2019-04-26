
# 
# External dependencies
# 

find_package(glm REQUIRED)
find_package(glbinding REQUIRED)

# 
# Library name and options
# 

# Target name
set(target globjects)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
set(feature_file         "include/${target}/${target}_features.h")
set(export_file          "include/${target}/${target}_export.h")
set(template_export_file "include/${target}/${target}_api.h")
set(export_macro         "${target_id}_API")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/AbstractState.h
    ${include_path}/AbstractState.inl
    ${include_path}/AbstractUniform.h
    ${include_path}/AbstractUniform.inl
    ${include_path}/Buffer.h
    ${include_path}/Buffer.inl
    ${include_path}/Capability.h
    ${include_path}/DebugMessage.h
    ${include_path}/Error.h
    ${include_path}/FramebufferAttachment.h
    ${include_path}/Framebuffer.h
    ${include_path}/glbindinglogging.h
    ${include_path}/glmlogging.h
    ${include_path}/globjects.h
    ${include_path}/globjects.inl
    ${include_path}/LocationIdentity.h
    ${include_path}/logging.h
    ${include_path}/NamedString.h
    ${include_path}/Object.h
    ${include_path}/objectlogging.h
    ${include_path}/objectlogging.inl
    ${include_path}/ProgramBinary.h
    ${include_path}/Program.h
    ${include_path}/Program.inl
    ${include_path}/ProgramPipeline.h
    ${include_path}/Query.h
    ${include_path}/AttachedRenderbuffer.h
    ${include_path}/Renderbuffer.h
    ${include_path}/Resource.h
    ${include_path}/Sampler.h
    ${include_path}/Shader.h
    ${include_path}/State.h
    ${include_path}/StateSetting.h
    ${include_path}/StateSetting.inl
    ${include_path}/Sync.h
    ${include_path}/AttachedTexture.h
    ${include_path}/Texture.h
    ${include_path}/TextureHandle.h
    ${include_path}/TransformFeedback.h
    ${include_path}/TransformFeedback.inl
    ${include_path}/UniformBlock.h
    ${include_path}/Uniform.h
    ${include_path}/Uniform.inl
    ${include_path}/VertexArray.h
    ${include_path}/VertexAttributeBinding.h
    
    ${include_path}/base/AbstractStringSource.h
    ${include_path}/base/AbstractFunctionCall.h
    ${include_path}/base/AbstractLogHandler.h
    ${include_path}/base/baselogging.h
    ${include_path}/base/CompositeStringSource.h
    ${include_path}/base/ConsoleLogger.h
    ${include_path}/base/File.h
    ${include_path}/base/FileRegistry.h
    ${include_path}/base/FunctionCall.h
    ${include_path}/base/FunctionCall.inl
    ${include_path}/base/Instantiator.h
    ${include_path}/base/Instantiator.inl
    ${include_path}/base/LogMessageBuilder.h
    ${include_path}/base/LogMessageBuilder.inl
    ${include_path}/base/LogMessage.h
    ${include_path}/base/LogMessageLevel.h
    ${include_path}/base/StaticStringSource.h
    ${include_path}/base/StringTemplate.h
)

set(sources
    ${source_path}/AbstractState.cpp
    ${source_path}/AbstractUniform.cpp
    ${source_path}/Buffer.cpp
    ${source_path}/Capability.cpp
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
    ${source_path}/base/container_helpers.h
    ${source_path}/base/container_helpers.inl
    ${source_path}/base/CompositeStringSource.cpp
    ${source_path}/base/ConsoleLogger.cpp
    ${source_path}/base/File.cpp
    ${source_path}/base/FileRegistry.cpp
    ${source_path}/base/LogMessageBuilder.cpp
    ${source_path}/base/LogMessage.cpp
    ${source_path}/base/Singleton.h
    ${source_path}/base/Singleton.inl
    ${source_path}/base/StaticStringSource.cpp
    ${source_path}/base/StringTemplate.cpp

    ${source_path}/implementations/AbstractBufferImplementation.cpp
    ${source_path}/implementations/AbstractBufferImplementation.h

    ${source_path}/implementations/AbstractObjectNameImplementation.h
    ${source_path}/implementations/AbstractObjectNameImplementation.cpp
    ${source_path}/implementations/ObjectNameImplementation_DebugKHR.h
    ${source_path}/implementations/ObjectNameImplementation_DebugKHR.cpp
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
    ${source_path}/implementations/ShadingLanguageIncludeImplementation_ShadingLanguageIncludeARB.cpp
    ${source_path}/implementations/ShadingLanguageIncludeImplementation_ShadingLanguageIncludeARB.h
    ${source_path}/implementations/ShadingLanguageIncludeImplementation_Fallback.cpp
    ${source_path}/implementations/ShadingLanguageIncludeImplementation_Fallback.h
        
    ${source_path}/implementations/AbstractTextureImplementation.cpp
    ${source_path}/implementations/AbstractTextureImplementation.h
    ${source_path}/implementations/TextureImplementation_DirectStateAccessARB.cpp
    ${source_path}/implementations/TextureImplementation_DirectStateAccessARB.h
    ${source_path}/implementations/TextureImplementation_DirectStateAccessEXT.cpp
    ${source_path}/implementations/TextureImplementation_DirectStateAccessEXT.h
    ${source_path}/implementations/TextureImplementation_Legacy.cpp
    ${source_path}/implementations/TextureImplementation_Legacy.h
    
    ${source_path}/implementations/AbstractTextureStorageImplementation.cpp
    ${source_path}/implementations/AbstractTextureStorageImplementation.h
    ${source_path}/implementations/TextureStorageImplementation_DirectStateAccessARB.cpp
    ${source_path}/implementations/TextureStorageImplementation_DirectStateAccessARB.h
    ${source_path}/implementations/TextureStorageImplementation_DirectStateAccessEXT.cpp
    ${source_path}/implementations/TextureStorageImplementation_DirectStateAccessEXT.h
    ${source_path}/implementations/TextureStorageImplementation_Legacy.cpp
    ${source_path}/implementations/TextureStorageImplementation_Legacy.h
    ${source_path}/implementations/TextureStorageImplementation_Fallback.cpp
    ${source_path}/implementations/TextureStorageImplementation_Fallback.h

    ${source_path}/implementations/AbstractTextureStorageMultisampleImplementation.cpp
    ${source_path}/implementations/AbstractTextureStorageMultisampleImplementation.h
    ${source_path}/implementations/TextureStorageMultisampleImplementation_DirectStateAccessARB.cpp
    ${source_path}/implementations/TextureStorageMultisampleImplementation_DirectStateAccessARB.h
    ${source_path}/implementations/TextureStorageMultisampleImplementation_DirectStateAccessEXT.cpp
    ${source_path}/implementations/TextureStorageMultisampleImplementation_DirectStateAccessEXT.h
    ${source_path}/implementations/TextureStorageMultisampleImplementation_Legacy.cpp
    ${source_path}/implementations/TextureStorageMultisampleImplementation_Legacy.h
    ${source_path}/implementations/TextureStorageMultisampleImplementation_Fallback.cpp
    ${source_path}/implementations/TextureStorageMultisampleImplementation_Fallback.h

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
    ${source_path}/NamedString.cpp
    ${source_path}/Object.cpp
    ${source_path}/objectlogging.cpp
    ${source_path}/pixelformat.cpp
    ${source_path}/pixelformat.h
    ${source_path}/ProgramBinary.cpp
    ${source_path}/Program.cpp
    ${source_path}/ProgramPipeline.cpp
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
    ${source_path}/Sampler.cpp
    ${source_path}/Shader.cpp
    ${source_path}/State.cpp
    ${source_path}/StateSetting.cpp
    ${source_path}/Sync.cpp
    ${source_path}/AttachedTexture.cpp
    ${source_path}/Texture.cpp
    ${source_path}/TextureHandle.cpp
    ${source_path}/TransformFeedback.cpp
    ${source_path}/UniformBlock.cpp
    ${source_path}/VertexArray.cpp
    ${source_path}/VertexAttributeBinding.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.inl$" 
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.inl$" 
    ${source_group} ${sources})


# 
# Create library
# 

# Build library
add_library(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)

# Create API export header
generate_export_header(${target}
    EXPORT_FILE_NAME  ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
)

generate_template_export_header(${target}
    ${target_id}
    ${template_export_file}
)

# Create feature detection header
if (WriterCompilerDetectionHeaderFound)
    write_compiler_detection_header(
        FILE ${feature_file}
        PREFIX ${target_id}
        COMPILERS AppleClang Clang GNU MSVC
        FEATURES cxx_thread_local
        VERSION 3.2
    )
else()
    file(
        COPY ${PROJECT_SOURCE_DIR}/source/codegeneration/${target}_features.h
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/${target}
        USE_SOURCE_PERMISSIONS
    )
endif()


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    glm
    glbinding::glbinding
    glbinding::glbinding-aux

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    GLM_FORCE_RADIANS
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}
    $<$<BOOL:${OPTION_ERRORS_AS_EXCEPTION}>:GLOBJECTS_GL_ERROR_RAISE_EXCEPTION>

    INTERFACE
)

# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


#
# Target Health
#

perform_health_checks(
    ${target}
    ${sources}
    ${headers}
)


# 
# Deployment
# 

# Library
install(TARGETS ${target}
    EXPORT  "${target}-export"            COMPONENT dev
    RUNTIME DESTINATION ${INSTALL_BIN}    COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
    ARCHIVE DESTINATION ${INSTALL_LIB}    COMPONENT dev
)

# Header files
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# Generated header files
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# CMake config
install(EXPORT ${target}-export
    NAMESPACE   ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${target}
    COMPONENT   dev
)
