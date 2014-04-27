#include <glow/Sync.h>
#include <glow/Error.h>

namespace glow
{

Sync * Sync::fence(GLenum condition, GLbitfield flags)
{
    return new Sync(fenceSync(condition, flags));
}

Sync::Sync(GLsync sync)
: m_sync(sync)
{
}

Sync::~Sync()
{
    glDeleteSync(m_sync);
}

GLsync Sync::fenceSync(GLenum condition, GLbitfield flags)
{
    GLsync sync = glFenceSync(condition, flags);
    CheckGLError();

    return sync;
}

GLenum Sync::clientWait(GLbitfield flags, GLuint64 timeout)
{
    GLenum result = glClientWaitSync(m_sync, flags, timeout);
    CheckGLError();

    return result;
}

void Sync::wait(GLbitfield flags, GLuint64 timeout)
{
    glWaitSync(m_sync, flags, timeout);
    CheckGLError();
}

void Sync::get(GLenum pname, GLsizei bufsize, GLsizei * length, GLint * values)
{
    glGetSynciv(m_sync, pname, bufsize, length, values);
    CheckGLError();
}

GLint Sync::get(GLenum pname)
{
    GLint result = 0;
    get(pname, sizeof(result), nullptr, &result);

    return result;
}

} // namespace glow
