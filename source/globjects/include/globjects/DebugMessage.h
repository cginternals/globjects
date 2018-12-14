
#pragma once


#include <string>
#include <vector>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects
{


class GLOBJECTS_API DebugMessage
{
public:
    enum class Implementation
    {
        Legacy,
        DebugKHR
    };

    static void hintImplementation(Implementation impl);


public:
    using Callback = std::function<void(const DebugMessage &)>;

    DebugMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message);

    gl::GLenum source() const;
    gl::GLenum type() const;
    gl::GLuint id() const;
    gl::GLenum severity() const;
    const std::string & message() const;

    virtual std::string toString() const;


protected:
    gl::GLenum m_source;
    gl::GLenum m_type;
    gl::GLuint m_id;
    gl::GLenum m_severity;
    std::string m_message;

    std::string severityString() const;
    std::string sourceString() const;
    std::string typeString() const;


public:
    static void enable(bool synchronous = true);
    static void disable();
    static bool isEnabled();

    static void setCallback(Callback callback);

    static void setSynchronous(bool synchronous);
    static bool isSynchronous();

    static void insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message);
    static void insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message);
    static void insertMessage(const DebugMessage & message);

    static void enableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id);
    static void enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids);
    static void enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids);

    static void disableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id);
    static void disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids);
    static void disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids);

    static void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled);

    static bool isFallbackImplementation();
};


} // namespace globjects
