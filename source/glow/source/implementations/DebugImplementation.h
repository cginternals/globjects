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
    virtual void controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled) override;

protected:
    void registerCallback();

    static void APIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, const void * param);
};

} // namespace glow
