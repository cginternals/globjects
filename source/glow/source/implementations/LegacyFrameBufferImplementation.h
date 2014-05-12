#pragma once



#include <glow/Buffer.h>

#include "AbstractFrameBufferImplementation.h"

namespace glow
{

class LegacyFrameBufferImplementation : public AbstractFrameBufferImplementation
{
public:
    static gl::GLenum s_workingTarget;

    virtual gl::GLenum checkStatus(const FrameBufferObject * fbo) const override;
    virtual void setParameter(const FrameBufferObject * fbo, gl::GLenum pname, gl::GLint param) const override;
    virtual gl::GLint getAttachmentParameter(const FrameBufferObject * fbo, gl::GLenum attachment, gl::GLenum pname) const override;
    virtual void attachTexture(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const override;
    virtual void attachTexture1D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const override;
    virtual void attachTexture2D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level) const override;
    virtual void attachTexture3D(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const override;
    virtual void attachTextureLayer(const FrameBufferObject * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer) const override;
    virtual void attachRenderBuffer(const FrameBufferObject * fbo, gl::GLenum attachment, RenderBufferObject * renderBuffer) const override;
    virtual void setReadBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffer(const FrameBufferObject * fbo, gl::GLenum mode) const override;
    virtual void setDrawBuffers(const FrameBufferObject * fbo, gl::GLsizei n, const gl::GLenum * modes) const override;
};

} // namespace glow
