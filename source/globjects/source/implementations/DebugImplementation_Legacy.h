
#pragma once


#include "../base/Singleton.h"

#include "AbstractDebugImplementation.h"


namespace globjects 
{


class DebugImplementation_Legacy : public AbstractDebugImplementation
    , public Singleton<DebugImplementation_Legacy>
{
public:
    DebugImplementation_Legacy();

    virtual bool isFallback() override;

    virtual void enable() override;
    virtual void disable() override;
    virtual bool isEnabled() const override;
    virtual void setSynchronous(bool synchronous) override;
    virtual bool isSynchronous() const override;
    virtual void insertMessage(const DebugMessage & message) override;
    virtual void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled) override;


protected:
    bool m_enabled;
};


} // namespace globjects
