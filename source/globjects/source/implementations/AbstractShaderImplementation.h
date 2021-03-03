
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Shader.h>


namespace globjects
{


class AbstractShaderImplementation
{
public:
    AbstractShaderImplementation();
    virtual ~AbstractShaderImplementation();

    static AbstractShaderImplementation * get(Shader::Implementation impl =
        Shader::Implementation::Legacy);

    virtual gl::GLuint create(gl::GLenum type) const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
