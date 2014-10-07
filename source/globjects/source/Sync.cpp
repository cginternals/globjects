#include <globjects/Sync.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>


using namespace gl;

namespace globjects
{

Sync * Sync::fence(GLenum condition)
{
    return fence(condition, GL_UNUSED_BIT);
}

Sync * Sync::fence(GLenum condition, UnusedMask flags)
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

GLsync Sync::fenceSync(GLenum condition, UnusedMask flags)
{
    return glFenceSync(condition, flags);
}

GLsync Sync::sync() const
{
    return m_sync;
}

GLenum Sync::clientWait(SyncObjectMask flags, GLuint64 timeout)
{
    return glClientWaitSync(m_sync, flags, timeout);
}

void Sync::wait(GLuint64 timeout)
{
    wait(GL_UNUSED_BIT, timeout);
}

void Sync::wait(UnusedMask flags, GLuint64 timeout)
{
    glWaitSync(m_sync, flags, timeout);
}

void Sync::get(GLenum pname, GLsizei bufsize, GLsizei * length, GLint * values)
{
    glGetSynciv(m_sync, pname, bufsize, length, values);
}

GLint Sync::get(GLenum pname)
{
    GLint result = 0;
    get(pname, sizeof(result), nullptr, &result);

    return result;
}

} // namespace globjects
