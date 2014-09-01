#pragma once

#include <string>
#include <vector>
#include <array>

#include <glbinding/gl/types.h>
#include <glbinding/gl/extension.h>

#include <glbinding/Version.h>
#include <glbinding/ContextHandle.h>

#include <globjects/globjects_api.h>

namespace glo 
{

class AbstractStringSource;

GLOBJECTS_API void init();

GLOBJECTS_API void registerCurrentContext();
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
GLOBJECTS_API gl::GLint majorVersion();
GLOBJECTS_API gl::GLint minorVersion();
GLOBJECTS_API glbinding::Version version();
GLOBJECTS_API bool isCoreProfile();

GLOBJECTS_API std::vector<std::string> getExtensions();

template <int Count>
std::array<gl::GLint, Count> getIntegers(gl::GLenum pname);
template <int Count>
std::array<gl::GLfloat, Count> getFloats(gl::GLenum pname);
template <int Count>
std::array<gl::GLdouble, Count> getDoubles(gl::GLenum pname);
template <int Count>
std::array<gl::GLboolean, Count> getBooleans(gl::GLenum pname);

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

} // namespace glo

#include <globjects/globjects.hpp>
