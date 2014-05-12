#pragma once

#include <string>
#include <vector>
#include <array>

#include <glbinding/types.h>
#include <glbinding/Extension.h>

#include <glowbase/Version.h>

#include <glow/glow_api.h>

namespace glow 
{

class AbstractStringSource;

GLOW_API bool isInitialized();
GLOW_API bool init(bool showWarnings = false);

GLOW_API std::string getString(gl::GLenum pname);
GLOW_API std::string getString(gl::GLenum pname, gl::GLuint index);

GLOW_API gl::GLint getInteger(gl::GLenum pname);
GLOW_API gl::GLenum getEnum(gl::GLenum pname);
GLOW_API gl::GLfloat getFloat(gl::GLenum pname);
GLOW_API gl::GLdouble getDouble(gl::GLenum pname);
GLOW_API gl::GLboolean getBoolean(gl::GLenum pname);

GLOW_API gl::GLint getInteger(gl::GLenum pname, gl::GLuint index);
GLOW_API gl::GLenum getEnum(gl::GLenum pname, gl::GLuint index);
GLOW_API gl::GLfloat getFloat(gl::GLenum pname, gl::GLuint index);
GLOW_API gl::GLdouble getDouble(gl::GLenum pname, gl::GLuint index);
GLOW_API gl::GLboolean getBoolean(gl::GLenum pname, gl::GLuint index);

GLOW_API std::string vendor();
GLOW_API std::string renderer();
GLOW_API std::string versionString();
GLOW_API gl::GLint majorVersion();
GLOW_API gl::GLint minorVersion();
GLOW_API Version version();
GLOW_API bool isCoreProfile();

GLOW_API std::vector<std::string> getExtensions();

template <int Count>
std::array<gl::GLint, Count> getIntegers(gl::GLenum pname);
template <int Count>
std::array<gl::GLfloat, Count> getFloats(gl::GLenum pname);
template <int Count>
std::array<gl::GLdouble, Count> getDoubles(gl::GLenum pname);
template <int Count>
std::array<gl::GLboolean, Count> getBooleans(gl::GLenum pname);

GLOW_API bool hasExtension(gl::Extension extension);
GLOW_API bool hasExtension(const std::string & extensionName);
GLOW_API bool isInCoreProfile(gl::Extension extension, const Version & version);
GLOW_API bool isInCoreProfile(gl::Extension extension);

GLOW_API void enable(gl::GLenum capability);
GLOW_API void disable(gl::GLenum capability);
GLOW_API bool isEnabled(gl::GLenum capability);
GLOW_API void setEnabled(gl::GLenum capability, bool enabled);
GLOW_API void enable(gl::GLenum capability, int index);
GLOW_API void disable(gl::GLenum capability, int index);
GLOW_API bool isEnabled(gl::GLenum capability, int index);
GLOW_API void setEnabled(gl::GLenum capability, int index, bool enabled);

} // namespace glow

#include <glow/glow.hpp>
