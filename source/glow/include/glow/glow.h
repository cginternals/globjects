#pragma once

#include <string>
#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Version.h>
#include <glow/Extension.h>

namespace glow 
{

class AbstractStringSource;

GLOW_API bool isInitialized();
GLOW_API bool init(bool showWarnings = false);

GLOW_API std::string getString(GLenum pname);
GLOW_API std::string getString(GLenum pname, GLuint index);

GLOW_API GLint getInteger(GLenum pname);
GLOW_API GLenum getEnum(GLenum pname);
GLOW_API GLfloat getFloat(GLenum pname);
GLOW_API GLdouble getDouble(GLenum pname);
GLOW_API GLboolean getBoolean(GLenum pname);

GLOW_API GLint getInteger(GLenum pname, GLuint index);
GLOW_API GLenum getEnum(GLenum pname, GLuint index);
GLOW_API GLfloat getFloat(GLenum pname, GLuint index);
GLOW_API GLdouble getDouble(GLenum pname, GLuint index);
GLOW_API GLboolean getBoolean(GLenum pname, GLuint index);

GLOW_API std::string vendor();
GLOW_API std::string renderer();
GLOW_API std::string versionString();
GLOW_API GLint majorVersion();
GLOW_API GLint minorVersion();
GLOW_API Version version();
GLOW_API bool isCoreProfile();

GLOW_API std::vector<std::string> getExtensions();

template <int Count>
std::array<GLint, Count> getIntegers(GLenum pname);
template <int Count>
std::array<GLfloat, Count> getFloats(GLenum pname);
template <int Count>
std::array<GLdouble, Count> getDoubles(GLenum pname);
template <int Count>
std::array<GLboolean, Count> getBooleans(GLenum pname);

GLOW_API bool hasExtension(Extension extension);
GLOW_API bool hasExtension(const std::string & extensionName);
GLOW_API bool isInCoreProfile(Extension extension, const Version & version);
GLOW_API bool isInCoreProfile(Extension extension);

GLOW_API void enable(GLenum capability);
GLOW_API void disable(GLenum capability);
GLOW_API bool isEnabled(GLenum capability);
GLOW_API void setEnabled(GLenum capability, bool enabled);
GLOW_API void enable(GLenum capability, int index);
GLOW_API void disable(GLenum capability, int index);
GLOW_API bool isEnabled(GLenum capability, int index);
GLOW_API void setEnabled(GLenum capability, int index, bool enabled);

} // namespace glow

#include <glow/glow.hpp>
