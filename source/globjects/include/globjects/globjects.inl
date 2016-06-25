
#pragma once


#include <utility>

#include <glbinding/gl/functions.h>


namespace globjects 
{


template <int Count>
std::array<gl::GLint, Count> getIntegers(const gl::GLenum pname)
{
    std::array<gl::GLint, Count> values;

    gl::glGetIntegerv(pname, reinterpret_cast<gl::GLint*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLenum, Count> getEnums(const gl::GLenum pname)
{
    std::array<gl::GLenum, Count> values;

    gl::glGetIntegerv(pname, reinterpret_cast<gl::GLint*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLfloat, Count> getFloats(const gl::GLenum pname)
{
    std::array<gl::GLfloat, Count> values;

    gl::glGetFloatv(pname, reinterpret_cast<gl::GLfloat*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLdouble, Count> getDoubles(const gl::GLenum pname)
{
    std::array<gl::GLdouble, Count> values;

    gl::glGetDoublev(pname, reinterpret_cast<gl::GLdouble*>(&values));

    return values;
}

template <int Count>
std::array<gl::GLboolean, Count> getBooleans(const gl::GLenum pname)
{
    std::array<gl::GLboolean, Count> values;

    gl::glGetBooleanv(pname, reinterpret_cast<gl::GLboolean*>(&values));

    return values;
}

template <typename T, typename... Args>
void init(T strategy, Args... args)
{
    init(args...);

    initializeStrategy(strategy);
}

template <typename T, typename... Args>
void init(const glbinding::ContextHandle sharedContextId, T strategy, Args... args)
{
    init(sharedContextId, args...);

    initializeStrategy(strategy);
}


} // namespace globjects
