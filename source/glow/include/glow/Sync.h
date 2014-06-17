#pragma once

#include <glbinding/types.h>
#include <glbinding/constants.h>

#include <glowbase/Referenced.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API Sync : public Referenced
{
public:
    static Sync * fence(gl::GLenum condition = gl::GL_SYNC_GPU_COMMANDS_COMPLETE, gl::GLbitfield flags = 0);

    virtual ~Sync();

    gl::GLenum clientWait(gl::GLbitfield flags = gl::GL_SYNC_FLUSH_COMMANDS_BIT, gl::GLuint64 timeout = gl::GL_TIMEOUT_IGNORED);
    void wait(gl::GLbitfield flags = 0, gl::GLuint64 timeout = gl::GL_TIMEOUT_IGNORED);

    void get(gl::GLenum pname, gl::GLsizei bufsize, gl::GLsizei * length, gl::GLint * values);
    gl::GLint get(gl::GLenum pname);

    gl::GLsync sync() const;
protected:
    gl::GLsync m_sync;

    Sync(gl::GLsync sync);

    static gl::GLsync fenceSync(gl::GLenum condition, gl::GLbitfield flags);
};

} // namespace glow
