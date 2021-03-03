
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Program.h>


namespace globjects
{


class AbstractProgramImplementation
{
public:
    AbstractProgramImplementation();
    virtual ~AbstractProgramImplementation();

    static AbstractProgramImplementation * get(Program::Implementation impl =
        Program::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
