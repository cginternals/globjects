#pragma once

#include <string>

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API DebugMessage
{
public:
    DebugMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message);

    gl::GLenum source() const;
    gl::GLenum type() const;
    gl::GLuint id() const;
    gl::GLenum severity() const;
    const std::string & message() const;

    virtual bool isManualErrorMessage() const;

    std::string severityString() const;
    std::string sourceString() const;
    std::string typeString() const;

    virtual std::string toString() const;

protected:
    gl::GLenum m_source;
    gl::GLenum m_type;
    gl::GLuint m_id;
    gl::GLenum m_severity;
    std::string m_message;
};

} // namespace glow
