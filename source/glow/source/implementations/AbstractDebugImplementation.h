#pragma once

#include <GL/glew.h>

#include <functional>
#include <vector>

#include <glow/DebugMessage.h>
#include "../DebugMessageCallback.h"

namespace glow {

class AbstractDebugImplementation
{
public:
    AbstractDebugImplementation();
    virtual ~AbstractDebugImplementation();

    static AbstractDebugImplementation * create();

    void setCallback(DebugMessageCallback::Callback callback);
    void addCallback(DebugMessageCallback::Callback callback);

    virtual bool isFallback();

    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void setSynchronous(bool synchronous) = 0;

    virtual void insertMessage(const DebugMessage & message) = 0;

    virtual void controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled) = 0;
protected:
    DebugMessageCallback m_messageCallback;
};

} // namespace glow
