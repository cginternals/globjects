
#pragma once


#include <glbinding/gl/types.h>

#include "../base/Singleton.h"

#include "AbstractDebugImplementation.h"


namespace globjects 
{


class DebugImplementation_DebugKHR : public AbstractDebugImplementation
    , public Singleton<DebugImplementation_DebugKHR>
{
public:
    DebugImplementation_DebugKHR();

    virtual void enable() override;
    virtual void disable() override;
    virtual bool isEnabled() const override;
    virtual void setSynchronous(bool synchronous) override;
    virtual bool isSynchronous() const override;
    virtual void insertMessage(const DebugMessage & message) override;
    virtual void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled) override;


protected:
    bool m_isRegistered;
    void registerCallback();

    static void GL_APIENTRY debugMessageCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message, const void * param);
};


} // namespace globjects
