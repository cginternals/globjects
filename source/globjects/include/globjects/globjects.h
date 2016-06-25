
#pragma once

#include <string>
#include <array>
#include <set>

#include <glbinding/gl/types.h>

#include <glbinding/ContextHandle.h>

#include <globjects/globjects_api.h>

#include <globjects/AbstractUniform.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/DebugMessage.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Object.h>
#include <globjects/VertexArray.h>


namespace glbinding
{


class Version;


} // namespace glbinding


namespace globjects 
{


GLOBJECTS_API void init();
GLOBJECTS_API void init(glbinding::ContextHandle sharedContextId);

/** \brief calls detach on every registered object
    
    This only releases the GPU counterparts of all  objects, 
    while leaving the ref_ptred objects in memory, invalidated.
*/
GLOBJECTS_API void detachAllObjects();

template <typename T, typename... Args>
void init(T strategy, Args... args);

template <typename T, typename... Args>
void init(glbinding::ContextHandle sharedContextId, T strategy, Args... args);

GLOBJECTS_API void registerCurrentContext();
GLOBJECTS_API void registerCurrentContext(glbinding::ContextHandle sharedContextId);
GLOBJECTS_API void setContext(glbinding::ContextHandle contextId);
GLOBJECTS_API void setCurrentContext();

GLOBJECTS_API std::string getString(gl::GLenum pname);
GLOBJECTS_API std::string getString(gl::GLenum pname, gl::GLuint index);

GLOBJECTS_API gl::GLint getInteger(gl::GLenum pname);
GLOBJECTS_API gl::GLenum getEnum(gl::GLenum pname);
GLOBJECTS_API gl::GLfloat getFloat(gl::GLenum pname);
GLOBJECTS_API gl::GLdouble getDouble(gl::GLenum pname);
GLOBJECTS_API gl::GLboolean getBoolean(gl::GLenum pname);

GLOBJECTS_API gl::GLint getInteger(gl::GLenum pname, gl::GLuint index);
GLOBJECTS_API gl::GLenum getEnum(gl::GLenum pname, gl::GLuint index);
GLOBJECTS_API gl::GLfloat getFloat(gl::GLenum pname, gl::GLuint index);
GLOBJECTS_API gl::GLdouble getDouble(gl::GLenum pname, gl::GLuint index);
GLOBJECTS_API gl::GLboolean getBoolean(gl::GLenum pname, gl::GLuint index);

GLOBJECTS_API std::string vendor();
GLOBJECTS_API std::string renderer();
GLOBJECTS_API std::string versionString();
GLOBJECTS_API glbinding::Version version();
GLOBJECTS_API bool isCoreProfile();

template <int Count>
std::array<gl::GLint, Count> getIntegers(gl::GLenum pname);
template <int Count>
std::array<gl::GLenum, Count> getEnums(gl::GLenum pname);
template <int Count>
std::array<gl::GLfloat, Count> getFloats(gl::GLenum pname);
template <int Count>
std::array<gl::GLdouble, Count> getDoubles(gl::GLenum pname);
template <int Count>
std::array<gl::GLboolean, Count> getBooleans(gl::GLenum pname);

GLOBJECTS_API const std::set<gl::GLextension> & availableExtensions();
GLOBJECTS_API const std::set<std::string> & unknownAvailableExtensions();

GLOBJECTS_API bool hasExtension(gl::GLextension extension);
GLOBJECTS_API bool hasExtension(const std::string & extensionName);
GLOBJECTS_API bool isInCoreProfile(gl::GLextension extension, const glbinding::Version & version);
GLOBJECTS_API bool isInCoreProfile(gl::GLextension extension);

GLOBJECTS_API void enable(gl::GLenum capability);
GLOBJECTS_API void disable(gl::GLenum capability);
GLOBJECTS_API bool isEnabled(gl::GLenum capability);
GLOBJECTS_API void setEnabled(gl::GLenum capability, bool enabled);
GLOBJECTS_API void enable(gl::GLenum capability, int index);
GLOBJECTS_API void disable(gl::GLenum capability, int index);
GLOBJECTS_API bool isEnabled(gl::GLenum capability, int index);
GLOBJECTS_API void setEnabled(gl::GLenum capability, int index, bool enabled);

GLOBJECTS_API void initializeStrategy(AbstractUniform::BindlessImplementation impl);
GLOBJECTS_API void initializeStrategy(Buffer::BindlessImplementation impl);
GLOBJECTS_API void initializeStrategy(Framebuffer::BindlessImplementation impl);
GLOBJECTS_API void initializeStrategy(DebugMessage::Implementation impl);
GLOBJECTS_API void initializeStrategy(Program::BinaryImplementation impl);
GLOBJECTS_API void initializeStrategy(Shader::IncludeImplementation impl);
GLOBJECTS_API void initializeStrategy(Object::NameImplementation impl);
GLOBJECTS_API void initializeStrategy(VertexArray::AttributeImplementation impl);


} // namespace globjects


#include <globjects/globjects.inl>
