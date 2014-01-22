#pragma once

#include <string>
#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Version.h>

namespace glow 
{

class StringSource;

GLOW_API bool isInitialized();
GLOW_API bool init(bool showWarnings = false);

GLOW_API std::string getString(GLenum pname);
GLOW_API std::string getString(GLenum pname, GLuint index);

GLOW_API GLint getInteger(GLenum pname);
GLOW_API GLfloat getFloat(GLenum pname);
GLOW_API GLdouble getDouble(GLenum pname);
GLOW_API GLboolean getBoolean(GLenum pname);

GLOW_API GLint getInteger(GLenum pname, GLuint index);
GLOW_API GLfloat getFloat(GLenum pname, GLuint index);
GLOW_API GLdouble getDouble(GLenum pname, GLuint index);
GLOW_API GLboolean getBoolean(GLenum pname, GLuint index);

std::string vendor();
std::string renderer();
std::string versionString();
GLint majorVersion();
GLint minorVersion();
Version version();
bool isCoreProfile();

GLOW_API std::vector<std::string> getExtensions();

template <int Count>
GLOW_API std::array<GLint, Count> getIntegers(GLenum pname);
template <int Count>
GLOW_API std::array<GLfloat, Count> getFloats(GLenum pname);
template <int Count>
GLOW_API std::array<GLdouble, Count> getDoubles(GLenum pname);
template <int Count>
GLOW_API std::array<GLboolean, Count> getBooleans(GLenum pname);

GLOW_API void createNamedString(const std::string& name, const std::string& string, GLenum type = GL_SHADER_INCLUDE_ARB);
GLOW_API void createNamedString(const std::string& name, StringSource* source, GLenum type = GL_SHADER_INCLUDE_ARB);
GLOW_API void deleteNamedString(const std::string& name);
GLOW_API bool isNamedString(const std::string& name, bool cached = false);
GLOW_API std::string getNamedString(const std::string& name, bool cached = false);
GLOW_API StringSource* getNamedStringSource(const std::string& name);
GLOW_API GLenum getNamedStringType(const std::string& name, bool cached = false);

GLOW_API void enable(GLenum capability);
GLOW_API void disable(GLenum capability);
GLOW_API bool isEnabled(GLenum capability);
GLOW_API void setEnabled(GLenum capability, bool enabled);
GLOW_API void enable(GLenum capability, int index);
GLOW_API void disable(GLenum capability, int index);
GLOW_API bool isEnabled(GLenum capability, int index);
GLOW_API void setEnabled(GLenum capability, int index, bool enabled);

} // namespace glow

#include <glow/global.hpp>
