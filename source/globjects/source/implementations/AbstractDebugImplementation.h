
#pragma once


#include <glbinding/gl/types.h>

#include <globjects/DebugMessage.h>


namespace globjects 
{


class AbstractDebugImplementation
{
public:
    AbstractDebugImplementation();
    virtual ~AbstractDebugImplementation();

    static AbstractDebugImplementation * get(DebugMessage::Implementation impl = 
        DebugMessage::Implementation::DebugKHR);


    void setCallback(DebugMessage::Callback callback);

    virtual bool isFallback();

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool isEnabled() const = 0;

    virtual void setSynchronous(bool synchronous) = 0;
    virtual bool isSynchronous() const = 0;

    virtual void insertMessage(const DebugMessage & message) = 0;

    virtual void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled) = 0;


protected:
    DebugMessage::Callback m_messageCallback;
    static DebugMessage::Callback s_defaultCallback;

    virtual void handleMessage(const DebugMessage & message) const;
};


} // namespace globjects
