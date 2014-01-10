#pragma once

#include <string>
#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Version.h>


namespace glow 
{

GLOW_API bool init();

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


template <int Count>
GLOW_API std::array<GLint, Count> getIntegers(GLenum pname);
template <int Count>
GLOW_API std::array<GLfloat, Count> getFloats(GLenum pname);
template <int Count>
GLOW_API std::array<GLdouble, Count> getDoubles(GLenum pname);
template <int Count>
GLOW_API std::array<GLboolean, Count> getBooleans(GLenum pname);

} // namespace glow

#include <glow/global.hpp>
