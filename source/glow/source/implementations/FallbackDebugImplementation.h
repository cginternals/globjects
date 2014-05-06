#pragma once

#include "AbstractDebugImplementation.h"

namespace glow {

class FallbackDebugImplementation : public AbstractDebugImplementation
{
public:
    FallbackDebugImplementation();

    virtual bool isFallback() override;

    virtual void enable() override;
    virtual void disable() override;
    virtual void setSynchronous(bool synchronous) override;
    virtual void insertMessage(const DebugMessage & message) override;
    virtual void controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled) override;
protected:
    bool m_enabled;
};

} // namespace glow
