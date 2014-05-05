#pragma once

#include <GL/glew.h>

#include <glowbase/Referenced.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API Sync : public Referenced
{
public:
    static Sync * fence(GLenum condition = GL_SYNC_GPU_COMMANDS_COMPLETE, GLbitfield flags = 0);

    virtual ~Sync();

    GLenum clientWait(GLbitfield flags = GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64 timeout = GL_TIMEOUT_IGNORED);
    void wait(GLbitfield flags = 0, GLuint64 timeout = GL_TIMEOUT_IGNORED);

    void get(GLenum pname, GLsizei bufsize, GLsizei * length, GLint * values);
    GLint get(GLenum pname);

    GLsync sync() const;
protected:
    GLsync m_sync;

    Sync(GLsync sync);

    static GLsync fenceSync(GLenum condition, GLbitfield flags);
};

} // namespace glow
