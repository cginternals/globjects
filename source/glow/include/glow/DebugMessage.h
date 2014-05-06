#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Error.h>

namespace glow
{

class GLOW_API DebugMessage
{
public:
    DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string & message);

    GLenum source() const;
    GLenum type() const;
    GLuint id() const;
    GLenum severity() const;
    const std::string & message() const;

    virtual bool isManualErrorMessage() const;

    std::string severityString() const;
    std::string sourceString() const;
    std::string typeString() const;

    virtual std::string toString() const;

protected:
    GLenum m_source;
    GLenum m_type;
    GLuint m_id;
    GLenum m_severity;
    std::string m_message;
};

} // namespace glow
