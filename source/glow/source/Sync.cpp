#include <glow/Sync.h>

#include <glbinding/functions.h>

namespace glow
{

Sync * Sync::fence(gl::GLenum condition, gl::GLbitfield flags)
{
    return new Sync(fenceSync(condition, flags));
}

Sync::Sync(gl::GLsync sync)
: m_sync(sync)
{
}

Sync::~Sync()
{
    gl::DeleteSync(m_sync);
}

gl::GLsync Sync::fenceSync(gl::GLenum condition, gl::GLbitfield flags)
{
    return gl::FenceSync(condition, flags);
}

gl::GLsync Sync::sync() const
{
    return m_sync;
}

gl::GLenum Sync::clientWait(gl::GLbitfield flags, gl::GLuint64 timeout)
{
    return gl::ClientWaitSync(m_sync, flags, timeout);
}

void Sync::wait(gl::GLbitfield flags, gl::GLuint64 timeout)
{
    gl::WaitSync(m_sync, flags, timeout);
}

void Sync::get(gl::GLenum pname, gl::GLsizei bufsize, gl::GLsizei * length, gl::GLint * values)
{
    gl::GetSynciv(m_sync, pname, bufsize, length, values);
}

gl::GLint Sync::get(gl::GLenum pname)
{
    gl::GLint result = 0;
    get(pname, sizeof(result), nullptr, &result);

    return result;
}

} // namespace glow
