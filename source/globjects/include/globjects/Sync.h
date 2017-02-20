
#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/base/Instantiator.h>


namespace globjects
{


class GLOBJECTS_API Sync : public Instantiator<Sync>
{
    friend class AbstractObjectNameImplementation;

public:
    static std::unique_ptr<Sync> fence(gl::GLenum condition);

    Sync(gl::GLsync sync);
    virtual ~Sync();

    gl::GLenum clientWait(gl::SyncObjectMask flags, gl::GLuint64 timeout);
    void wait(gl::GLuint64 timeout);

    void get(gl::GLenum pname, gl::GLsizei bufsize, gl::GLsizei * length, gl::GLint * values);
    gl::GLint get(gl::GLenum pname);

    gl::GLsync sync() const;

protected:
    void wait(gl::UnusedMask flags, gl::GLuint64 timeout);

    static gl::GLsync fenceSync(gl::GLenum condition, gl::UnusedMask flags);
    static std::unique_ptr<Sync> fence(gl::GLenum condition, gl::UnusedMask flags);

protected:
    gl::GLsync m_sync;
    mutable void * m_objectLabelState;
};


} // namespace globjects
