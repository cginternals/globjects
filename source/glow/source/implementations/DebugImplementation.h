#pragma once

#include "AbstractDebugImplementation.h"

#ifdef WIN32
#include <Windows.h>
#else
#define APIENTRY
#endif

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

    static void APIENTRY debugMessageCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message, const void * param);
};

} // namespace glow
