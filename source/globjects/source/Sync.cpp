#include <globjects/Sync.h>

#include <glbinding/gl/functions.h>

namespace glo
{

Sync * Sync::fence(gl::GLenum condition, gl::UnusedMask flags)
{
    return new Sync(fenceSync(condition, flags));
}

Sync::Sync(gl::GLsync sync)
: m_sync(sync)
{
}

Sync::~Sync()
{
    gl::glDeleteSync(m_sync);
}

gl::GLsync Sync::fenceSync(gl::GLenum condition, gl::UnusedMask flags)
{
    return gl::glFenceSync(condition, flags);
}

gl::GLsync Sync::sync() const
{
    return m_sync;
}

gl::GLenum Sync::clientWait(gl::SyncObjectMask flags, gl::GLuint64 timeout)
{
    return gl::glClientWaitSync(m_sync, flags, timeout);
}

void Sync::wait(gl::UnusedMask flags, gl::GLuint64 timeout)
{
    gl::glWaitSync(m_sync, flags, timeout);
}

void Sync::get(gl::GLenum pname, gl::GLsizei bufsize, gl::GLsizei * length, gl::GLint * values)
{
    gl::glGetSynciv(m_sync, pname, bufsize, length, values);
}

gl::GLint Sync::get(gl::GLenum pname)
{
    gl::GLint result = 0;
    get(pname, sizeof(result), nullptr, &result);

    return result;
}

} // namespace glo
