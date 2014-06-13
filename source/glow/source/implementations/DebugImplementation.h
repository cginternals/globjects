#pragma once

#include <glbinding/types.h>

#include "AbstractDebugImplementation.h"

namespace glow {

class DebugImplementation : public AbstractDebugImplementation
{
public:
    virtual void enable() override;
    virtual void disable() override;
    virtual void setSynchronous(bool synchronous) override;
    virtual void insertMessage(const DebugMessage & message) override;
    virtual void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled) override;

protected:
    void registerCallback();

    static void GL_APIENTRY debugMessageCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message, const void * param);
};

} // namespace glow
