
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <globjects/globjects_api.h>
#include <globjects/FramebufferAttachment.h>


namespace globjects 
{


class Texture;
class Framebuffer;

/** \brief Encapsulates texture attachments of a frame buffer object.
    
    This class is a link between a Texture and the FrameBufferObject it is attached to.
    
    \see Texture
    \see Framebuffer
 */
class GLOBJECTS_API AttachedTexture : public FramebufferAttachment
{
public:
    AttachedTexture(Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer = -1);

    virtual bool isTextureAttachment() const override;

    Texture * texture();
    const Texture * texture() const;

    gl::GLint level() const;

    bool hasLayer() const;
    gl::GLint layer() const;

protected:
    ref_ptr<Texture> m_texture;
    gl::GLint m_level;
    gl::GLint m_layer;
};


} // namespace globjects
